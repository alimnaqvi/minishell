/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vector.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rreimann <rreimann@student.42heilbronn.de> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/21 15:12:33 by rreimann          #+#    #+#             */
/*   Updated: 2025/02/13 15:39:47 by rreimann         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// So vector.elements is a pointer to a pointer
// So when we set it to NULL to begin with
// This means that there is no pointer it is pointing to
// Well, NULL is a void pointer, which means that it cannot be dereferenced
/*
	So when we have a double pointer
	...it is essentially a pointer to an array
	The array itself is essentially a pointer to something
	And it can be a pointer to pretty much anything
	
	So you can do *vector.elements
	and it would give you the pointer to the very first element in the array
	vector.elements is a pointer to the array itself
	
	when you do `(*vector.elements) + (vector.element_size)`
	...then you will get a pointer to the second element in the array of pointers
	And you can dereference it to get the actual element itself
	The element itself may also be just a pointer to an element, though
*/
t_vec	vec_init(size_t element_size)
{
	t_vec	vector;

	vector.length = 0;
	vector.element_size = element_size;
	vector.elements = NULL;
	return (vector);
}

// Increase the size of the vector by one
void	vec_increase_size(t_minishell *minishell, t_vec *vec)
{
	size_t	old_length;
	void	*new_array;

	old_length = vec->length;
	vec->length += 1;
	new_array = gc_malloc(vec->length * vec->element_size, minishell);
	if (vec->length > 1)
	{
		ft_memcpy(new_array, vec->elements, old_length * vec->element_size);
		gc_free(vec->elements, minishell);
	}
	vec->elements = new_array;
}

// Push an element to the env of the vector
void	vec_push_ref(t_minishell *minishell, t_vec *vec, void *element)
{
	void	*new_element_ref;

	vec_increase_size(minishell, vec);
	new_element_ref = vec->elements + ((vec->length - 1) * vec->element_size);
	ft_memcpy(new_element_ref, &element, vec->element_size);
}

void	vec_push_copy(t_minishell *minishell, t_vec *vec, void *element)
{
	void	*new_element_ref;

	vec_increase_size(minishell, vec);
	new_element_ref = vec->elements + ((vec->length - 1) * vec->element_size);
	ft_memcpy(new_element_ref, element, vec->element_size);
}

void	*vec_get(t_vec *vec, size_t index)
{
	if (index >= vec->length)
		return (NULL);
	return (vec->elements + (index * vec->element_size));
}
